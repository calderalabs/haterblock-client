type t = {
  id: int,
  body: string
};

module CommentDecoder =
  JsonApi.MakeDecoder(
    {
      type model = t;
      type attributes = {body: string};
      let decoder = (json: Js.Json.t) : attributes =>
        Json.Decode.{body: json |> field("body", string)};
      let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : model =>
        switch resource.attributes {
        | None => {id: resource.id, body: ""}
        | Some(attributes) => {id: resource.id, body: attributes.body}
        };
    }
  );

let fetchAll = (callback: array(t) => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=json => callback(json |> CommentDecoder.decodeMany),
    ()
  );
